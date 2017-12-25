#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#define MY_PORT 33333

struct nf_book_state;
struct iphdr;
struct tcphdr;

static unsigned int my_hook_fn_pre(void *priv, struct sk_buff *skb, const struct nf_book_state *state) {
    struct iphdr  *ih;
    struct tcphdr *th;
    unsigned int sport, dport;
    char saddr[16], daddr[16];

    if(!skb)
        return NF_DROP;

    if(!(ih = ip_hdr(skb)))
        return NF_DROP;

    if(ih->protocol != IPPROTO_TCP)
        return NF_ACCEPT;

    if(!(th = tcp_hdr(skb)))
        return NF_ACCEPT;

    snprintf(saddr, 16, "%pI4", &ih->saddr);
    snprintf(daddr, 16, "%pI4", &ih->daddr);
    sport = htons((unsigned short int) th->source);
    dport = htons((unsigned short int) th->dest);

    printk(KERN_INFO "PRE_ROUTING  (%5d, %5d, %s, %s)\n", sport, dport, saddr, daddr);

    if(sport == MY_PORT) {
        ih->saddr = ih->daddr;
        ih->daddr = ntohl(0x01020304);
        th->source = ntohs((unsigned short int) 7777);
        th->dest = ntohs((unsigned short int) 7777);
    }

    return NF_ACCEPT;
}

static unsigned int my_hook_fn_forward(void *priv, struct sk_buff *skb, const struct nf_book_state *state) {
    struct iphdr  *ih;
    struct tcphdr *th;
    unsigned int sport, dport;
    char saddr[16], daddr[16];

    if(!skb)
        return NF_DROP;

    if(!(ih = ip_hdr(skb)))
        return NF_DROP;

    if(ih->protocol != IPPROTO_TCP)
        return NF_ACCEPT;

    if(!(th = tcp_hdr(skb)))
        return NF_ACCEPT;

    snprintf(saddr, 16, "%pI4", &ih->saddr);
    snprintf(daddr, 16, "%pI4", &ih->daddr);

    sport = htons((unsigned short int) th->source);
    dport = htons((unsigned short int) th->dest);

    printk(KERN_INFO "FORWARD      (%5d, %5d, %s, %s)\n", sport, dport, saddr, daddr);

    return NF_ACCEPT;
}

static unsigned int my_hook_fn_post(void *priv, struct sk_buff *skb, const struct nf_book_state *state) {
    struct iphdr  *ih;
    struct tcphdr *th;
    unsigned int sport, dport;
    char saddr[16], daddr[16];

    if(!skb)
        return NF_DROP;

    if(!(ih = ip_hdr(skb)))
        return NF_DROP;

    if(ih->protocol != IPPROTO_TCP)
        return NF_ACCEPT;

    if(!(th = tcp_hdr(skb)))
        return NF_ACCEPT;

    snprintf(saddr, 16, "%pI4", &ih->saddr);
    snprintf(daddr, 16, "%pI4", &ih->daddr);

    sport = htons((unsigned short int) th->source);
    dport = htons((unsigned short int) th->dest);

    printk(KERN_INFO "POST_ROUTING (%5d, %5d, %s, %s)\n", sport, dport, saddr, daddr);

    return NF_ACCEPT;
}

static struct nf_hook_ops my_nf_ops_pre = {
    .hook = my_hook_fn_pre,
    .hooknum = NF_INET_PRE_ROUTING,
    .pf = PF_INET,
    .priority = NF_IP_PRI_FIRST,
};

static struct nf_hook_ops my_nf_ops_forward = {
    .hook = my_hook_fn_forward,
    .hooknum = NF_INET_PRE_ROUTING,
    .pf = PF_INET,
    .priority = NF_IP_PRI_FIRST,
};

static struct nf_hook_ops my_nf_ops_post = {
    .hook = my_hook_fn_post,
    .hooknum = NF_INET_PRE_ROUTING,
    .pf = PF_INET,
    .priority = NF_IP_PRI_FIRST,
};

static int __init simple_init(void) {
    printk(KERN_INFO "Simple Module Init!!\n");

    nf_register_hook(&my_nf_ops_pre);
    nf_register_hook(&my_nf_ops_forward);
    nf_register_hook(&my_nf_ops_post);

    return 0;
}

static void __exit simple_exit(void) {
    nf_unregister_hook(&my_nf_ops_pre);
    nf_unregister_hook(&my_nf_ops_forward);
    nf_unregister_hook(&my_nf_ops_post);
    printk(KERN_INFO "Simple Module Exit!!\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_AUTHOR("Korea University");
MODULE_DESCRIPTION("It's simple!!");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
